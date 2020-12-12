package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
)

type Ramjson struct {
	RamTotal  int `json:"RAM"`
	RamFree   int `json:"FREE"`
	RammUsada int `json:"USADA"`
}

type Cpujson struct {
	Total int `json:"Total"`
}

type Estadisticasjson struct {
	Total       string `json:"Total"`
	Ejecucion   string `json:"Ejecucion"`
	Suspendidos string `json:"suspendidos"`
	Detenidos   string `json:"detenidos"`
	Zombie      string `json:"zombies"`
}

type Datosjson struct {
	Pid           string      `json:"Pid"`
	Nombre        string      `json:"Nombre"`
	Usuario       string      `json:"Usuario"`
	Usuarionombre string      `json:"Usuarionombre"`
	Estado        string      `json:"Estado"`
	RAM           string      `json:"Estado"`
	Hijos         []Datosjson `json:"hijos"`
}

type Procesojson struct {
	Procesos []Datosjson      `json:"ProcessList"`
	Info     Estadisticasjson `json:"ProcessInfo"`
}

func main() {
	//Read txt

	http.HandleFunc("/", serverWeb)

	log.Println("Listening on :8080...")
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		log.Fatal(err)
	}
}

func serverWeb(w http.ResponseWriter, r *http.Request) {
	fmt.Fprint(w, "<h1>Hola mundo<h1>")
	fmt.Fprint(w, getRAM())
	fmt.Fprintln(w, getCPU())
	fmt.Fprintln(w, getPROCESO())
}

func getRAM() string {
	data, err := ioutil.ReadFile("/proc/memo_201503953")
	if err != nil {
		panic(err)
	}
	info := Ramjson{}
	if err := json.Unmarshal(data, &info); err != nil {
		fmt.Println("Error al deserealizar el json: ", err)
	} else {
		fmt.Println(info)
	}
	return string(data)
}

func getCPU() string {
	data, err := ioutil.ReadFile("/proc/cpu_201503953")
	if err != nil {
		panic(err)
	}
	info := Cpujson{}
	if err := json.Unmarshal(data, &info); err != nil {
		fmt.Println("Error al deserealizar el json: ", err)
	} else {
		fmt.Println(info)
	}
	return string(data)
}

func getPROCESO() string {
	data, err := ioutil.ReadFile("/proc/proceso_201503953")
	if err != nil {
		panic(err)
	}
	info := Procesojson{}
	if err := json.Unmarshal([]byte(data), &info); err != nil {
		fmt.Println("Error al deserealizar el json: ", err)
	} else {
		fmt.Println(info)
	}
	return string(data)
}
