package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"time"

	"github.com/gorilla/websocket"
)

var clients = make(map[*websocket.Conn]string)

type Message struct {
	Dato string `json:"dato"`
}

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

var upgrader = websocket.Upgrader{
	ReadBufferSize:  1024,
	WriteBufferSize: 1024,

	CheckOrigin: func(r *http.Request) bool { return true },
}

func serveWs(w http.ResponseWriter, r *http.Request) {
	log.Println("1--------------------------")
	fmt.Println(r.Host)

	ws, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Println("2--------------------")
		log.Println(err)
	}
	defer ws.Close()

	reader(ws)
}

func reader(conn *websocket.Conn) {
	for {
		messageType, p, err := conn.ReadMessage()
		if err != nil {
			log.Printf("Error: %v", err)
			delete(clients, conn)
			break
		}
		fmt.Println(string(p))
		clients[conn] = string(p)
		if err := conn.WriteMessage(messageType, p); err != nil {
			log.Println(err)
			return
		}
	}
}

func serverWeb(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintln(w, getPROCESO())
}

func serverRAM(w http.ResponseWriter, r *http.Request) {
	//fmt.Fprintln(w, getRAM())
	http.ServeFile(w, r, "ram.html")
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	fmt.Println("Endpoint: Memory Test")
	data, err := ioutil.ReadFile("/proc/memo_201503953")
	if err != nil {
		panic(err)
	}
	info := Cpujson{}
	json.Unmarshal(data, &info)
	json.NewEncoder(w).Encode(info)
}

func getRAM() *Ramjson {
	//upgrader.CheckOrigin = func(r *http.Request) bool { return true }
	data, err := ioutil.ReadFile("/proc/memo_201503953")
	if err != nil {
		panic(err)
	}
	strData := string(data)
	info := Ramjson{}
	if err := json.Unmarshal([]byte(strData), &info); err != nil {
		fmt.Println("Error al deserealizar el json: ", err)
	} else {
		ioutil.WriteFile("ram.json", []byte(strData), 0777)
		fmt.Println(info)
	}
	return &info
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

func setInfo() {
	for {
		for client := range clients {
			var value string = clients[client]
			log.Println(value)
			if value == "PRPCESO" {
				//por el momento nada
			} else if value == "RAM" {
				r := getRAM()
				if r != nil {
					errW := client.WriteJSON(r)
					if errW != nil {
						client.Close()
						delete(clients, client)
					}
				}
				//salidaLista := getRAM()
			}

		}
		r := getRAM()
		if r != nil {
			log.Println("Creando JSON")
		}
		fmt.Println(len(clients))
		log.Println("~~~~~~~~~~~~~~~~~")
		time.Sleep(4000 * time.Millisecond)
	}
}

func main() {
	//Read txt
	//http.Handle("/", fs)
	http.HandleFunc("/procesos.html", serverWeb)
	http.HandleFunc("/ram", serverRAM)
	http.HandleFunc("/cpu.html", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprint(w, getCPU())
	})
	go setInfo()

	log.Println("Listening on :8080...")
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		log.Fatal(err)
	}
}
