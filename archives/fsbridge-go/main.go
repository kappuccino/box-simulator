package main

import (
	"bytes"
	"fmt"
	"io"
	"log"
	"os"
	"strconv"
	"strings"
	"time"

	"github.com/jacobsa/go-serial/serial"
	sim "github.com/micmonay/simconnect"
)

var sc *sim.EasySimConnect

func main() {

	var err error
	sc, err = sim.NewEasySimConnect()
	if err != nil {
		fmt.Printf("err simconnect: %s", err)
		os.Exit(1)
	}

	sc.SetDelay(100 * time.Millisecond)

	var c <-chan bool
	for {
		c, err = sc.Connect("fsbridge")
		if err != nil {
			if err.Error() == "No connected" {
				fmt.Println("Can't connect to Flight Simulator. Is it launched ? I will retry in 1 second. Type 'ctrl C' to abort.")
				time.Sleep(1 * time.Second)
				continue
			}
			fmt.Printf("Error: %s", err)
			os.Exit(1)
		}
		break
	}
	<-c // Wait connection confirmation
	fmt.Println("connected to flight simulator")

	// set heading
	setHeading(227)
	//incrHeading()
	//incrHeading()
	//incrHeading()

	//os.Exit(0)

	// Set up options.
	options := serial.OpenOptions{
		PortName:        "COM3",
		BaudRate:        115200,
		DataBits:        8,
		StopBits:        1,
		MinimumReadSize: 4,
	}

	// Open the port.
	port, err := serial.Open(options)
	if err != nil {
		log.Fatalf("serial.Open: %v", err)
	}

	// Make sure to close it later.
	defer port.Close()
	var rx []byte
	minibuf := make([]byte, 1)
	flagStartSeen := false
	for {
		n, err := port.Read(minibuf)
		if n == 0 {
			continue
		}
		//fmt.Printf("n: %d\n", n)
		if err != nil {
			if err != io.EOF {
				fmt.Println("Error reading from serial port: ", err)
			}
		} else {
			//fmt.Printf("%v \n", minibuf[0])
			//fmt.Println(fmt.Sprintf("Rx: %s", rx))
			if minibuf[0] == 1 {
				rx = []byte{}
				flagStartSeen = true
				fmt.Println("flag seen seen")
				continue
			} else if minibuf[0] == 10 {
				if flagStartSeen {
					//fmt.Println(fmt.Sprintf("Rx: %s", rx))
					if err := execCmd(rx); err != nil {
						fmt.Printf("Error: %s\n", err)
					}
				}
				rx = []byte{}
			} else {
				rx = append(rx, minibuf[0])
			}
		}
	}
}

// parse and exec the received command
func execCmd(cmd []byte) error {
	p := bytes.SplitAfter(cmd, []byte{58})
	log.Printf("cmd: '%s' value: %s\n", p[0], p[1])

	if string(p[0]) != "E1:" {
		return nil
	}

	value, err := strconv.ParseInt(strings.TrimSpace(string(p[1])), 10, 64)
	if err != nil {
		return err
	}

	if value == 1 {
		incrHeading()
	} else {
		decrHeading()
	}

	// pour le poc on utilise le heading

	// on recupere la valeur courante
	/*heading, err := getHeading()
	if err != nil {
		return err
	}

	fmt.Printf("Heading: %d\n", heading)

	// on la modifie
	heading = heading + int(value)
	if heading > 360 {
		heading = heading - 360
	} else if heading < 0 {
		heading = 360 - heading
	}

	fmt.Printf("nouveau heading: %d\n", heading)

	// on l'injecte
	setHeading(heading)*/

	return nil
}

func getHeading() (heading int, err error) {
	var cSimvar <-chan []sim.SimVar
	cSimvar, err = sc.ConnectToSimVar(sim.SimVarAutopilotHeadingLockDir())
	if err != nil {
		return
	}
	value := <-cSimvar
	simVar := value[0]
	headingF, err := simVar.GetFloat64()
	if err != nil {
		return 0, err
	}
	heading = int(headingF)

	return
}

func setHeading(heading int) error {
	hdgSet := sc.NewSimEvent(sim.KeyHeadingBugSet)
	fmt.Println(<-hdgSet.RunWithValue(heading))
	return nil
}

func incrHeading() {
	evt := sc.NewSimEvent(sim.KeyHeadingBugInc)
	<-evt.Run()
}

func decrHeading() {
	evt := sc.NewSimEvent(sim.KeyHeadingBugDec)
	<-evt.Run()
}
