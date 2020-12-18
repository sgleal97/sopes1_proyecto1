package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os/exec"

	"github.com/gorilla/mux"
)

type Ramjson struct {
	RamTotal  int `json:"Total"`
	RamFree   int `json:"Libre"`
	RammUsada int `json:"Usada"`
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
	RAM           string      `json:"ram"`
	Hijos         []Datosjson `json:"hijos"`
}

type Procesojson struct {
	Procesos []Datosjson      `json:"ProcessList"`
	Info     Estadisticasjson `json:"ProcessInfo"`
}

//Obtener porcentaje de RAM
func getRAM(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	data, err := ioutil.ReadFile("/proc/memo_201503953")
	if err != nil {
		panic(err)
	}
	info := Ramjson{}
	json.Unmarshal(data, &info)
	json.NewEncoder(w).Encode(info)
}

//Obtener porcentaje de utilizacion
func getCPU(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	data, err := ioutil.ReadFile("/proc/cpu_201503953")
	if err != nil {
		panic(err)
	}
	info := Cpujson{}
	json.Unmarshal(data, &info)
	json.NewEncoder(w).Encode(info)
}

//Obtener todos los procesos
func getPROCESO(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	data, err := ioutil.ReadFile("/proc/proceso_201503953")
	if err != nil {
		panic(err)
	}
	info := Procesojson{}
	json.Unmarshal([]byte(data), &info)
	iterateArray(info.Procesos)
	json.NewEncoder(w).Encode(info)
}

func ramRoute(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Esta es la ram")
}

//Obtener nombre de usuario
func iterateArray(procesos []Datosjson) {
	for i := 0; i < len(procesos); i++ {
		current := procesos[i]
		user := current.Usuario
		cmd := "getent passwd " + user + " | cut -d: -f1"
		terminal := exec.Command("bash", "-c", cmd)
		output, err := terminal.CombinedOutput()
		if err != nil {
			fmt.Println(fmt.Sprint(err) + ": " + string(output))
		} else {
			procesos[i].Usuarionombre = string(output)
		}
		if len(current.Hijos) > 0 {
			iterateArray(current.Hijos)
		}
	}
}

func main() {
	router := mux.NewRouter().StrictSlash(true)

	router.HandleFunc("/", ramRoute)
	router.HandleFunc("/ram", getRAM).Methods("GET")
	router.HandleFunc("/cpu", getCPU).Methods("GET")
	router.HandleFunc("/process", getPROCESO).Methods("GET")
	log.Fatal(http.ListenAndServe(":3001", router))
}
