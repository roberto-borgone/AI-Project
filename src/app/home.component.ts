import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-home',
  template: '<h1 style="color:#3f51b5;" >Benvenuto in VirtualLabs</h1> <img style="height: 450px; width:auto; position: absolute; padding-left: 30vw;" src="./assets/images/prova.png"> <h3 style="color:#f16780;"> Sviluppato per il supporto ai laboratori virtuali </h3>' ,
  styles: ['h1 {padding: 32px 0 0 64px; font-size: 30px;} h3 {padding: 0 0 0 64px;}']
})
export class HomeComponent implements OnInit {

  constructor() { }

  ngOnInit(): void {
  }

}
