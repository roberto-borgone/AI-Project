import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-home',
  template: '<h2>Home</h2>',
  styles: ['h2 {padding: 32px 0 0 16px}']
})
export class HomeComponent implements OnInit {

  constructor() { }

  ngOnInit(): void {
  }

}
