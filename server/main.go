package main

//
// https://github.com/ayatothos/go-samples/blob/master/form_params/main.go
//

import (
	"fmt"
	"net/http"
	"io/ioutil"
	"os"
	"log"
)

func bindata_handler(w http.ResponseWriter, request *http.Request) {
	fmt.Println("bindata_handler")
	d, err := ioutil.ReadAll(request.Body)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
	}
	tmpfile, err := os.Create("./" + "data.bin")
	defer tmpfile.Close()
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
	}
	tmpfile.Write(d)

	w.WriteHeader(200)
	return
}

func main() {
	log.Println("Hello world.")
	http.HandleFunc("/sample1", post_rec)
	http.HandleFunc("/bindata", bindata_handler)
	http.HandleFunc("/sample3", Sample3)

	http.HandleFunc("/", func (w http.ResponseWriter, r *http.Request) {
        fmt.Fprintf(w, "Welcome to my website!")
    })

    fs := http.FileServer(http.Dir("static/"))
    http.Handle("/static/", http.StripPrefix("/static/", fs))

	port := 8000
	http.ListenAndServe(":8080", nil)
	http.ListenAndServe(fmt.Sprintf(":%d", port), nil)
}

func post_rec(w http.ResponseWriter, r *http.Request) {
	if err := r.ParseForm(); err != nil {
		fmt.Println("error")
	}

	//fmt.Println("hoge", r.FormValue("hoge"))
	//fmt.Println("foo", r.FormValue("foo"))
	for k, v := range r.Form {
		log.Printf("Form k=%v v=%v\n", k, v)
	}
}

func Sample3(w http.ResponseWriter, r *http.Request) {
	if err := r.ParseMultipartForm(32 << 20); err != nil {
		fmt.Println("error")
	}

	mf := r.MultipartForm

	for k, v := range mf.Value {
		fmt.Printf("%v : %v\n", k, v)
	}

	for k, v := range mf.File {
		for kk, vv := range v {
			fmt.Printf("%v : %v : %v\n", k, kk, vv.Filename)
			fmt.Printf("%v : %v : %v\n", k, kk, vv.Header)
			fmt.Printf("%v : %v : %v\n", k, kk, vv.Size)
		}
	}
}
