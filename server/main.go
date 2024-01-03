package main

//
// https://github.com/ayatothos/go-samples/blob/master/form_params/main.go
//

import (
	"fmt"
	"net/http"
)

func main() {
	fmt.Println(time.Now(), "Hello world.")
	http.HandleFunc("/sample1", post_rec)
	http.HandleFunc("/sample3", Sample3)

	http.HandleFunc("/", func (w http.ResponseWriter, r *http.Request) {
        fmt.Fprintf(w, "Welcome to my website!")
    })

    fs := http.FileServer(http.Dir("static/"))
    http.Handle("/static/", http.StripPrefix("/static/", fs))

	http.ListenAndServe(":8080", nil)
}

func post_rec(w http.ResponseWriter, r *http.Request) {
	if err := r.ParseForm(); err != nil {
		fmt.Println("error")
	}

	//fmt.Println("hoge", r.FormValue("hoge"))
	//fmt.Println("foo", r.FormValue("foo"))
	for k, v := range r.Form {
		fmt.Printf("%v : %v\n", k, v)
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
