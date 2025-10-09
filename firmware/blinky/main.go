// tinygo flash -target arduino-mega2560 -monitor -baudrate 9600 ./main.go

package main

import (
  "time"
  "machine"
)

func main() {
  led := machine.LED
  led.Configure(machine.PinConfig{Mode: machine.PinOutput})
  count := 0
  for {
    led.Low()
    println(count, ": Hello, World")
    time.Sleep(time.Millisecond * 500)
    led.High()
    time.Sleep(time.Millisecond * 500)
    count++
  }
}
