import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-page-not-found',
  template: '<h2>Page not found</h2>',
  styles: ['h2 {padding: 32px 0 0 16px }']
})
export class PageNotFoundComponent implements OnInit {

  constructor() { }

  ngOnInit(): void {
  }

}
