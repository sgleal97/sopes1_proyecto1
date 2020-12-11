package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

func main() {
	//Read txt
	data, err := ioutil.ReadFile("/proc/memo_201503953")

	if err != nil {
		panic(err)
	}

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprint(w, "<h1>Hola mundo<h1>")
		fmt.Fprint(w, "<h1>"+string(data)+"<h1>")
	})
	http.ListenAndServe(":8080", nil)
}
