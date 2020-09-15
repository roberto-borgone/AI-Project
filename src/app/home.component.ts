import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-home',
  template: '<h1 style="color:SlateBlue;" >Benvenuto in VirtualLabs</h1> <img class= center src="./assets/images/hero-img.png"> <h3 style="color:SlateBlue;"> Sviluppato per il supporto ai laboratori virtuali </h3>' ,
  styles: ['h1 {padding: 32px 0 0 16px; text-align: center;} h3 {padding: 32px 0 0 16px; text-align: center;}']
})
export class HomeComponent implements OnInit {

  constructor() { }

  ngOnInit(): void {
  }

}
