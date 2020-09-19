import { Component, Inject, OnInit, ViewChild } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';

@Component({
  selector: 'app-content-dialog',
  templateUrl: './content-dialog.component.html',
  styleUrls: ['./content-dialog.component.css']
})
export class ContentDialogComponent implements OnInit {

  url: any;
  
  zoom = "100%"


  constructor(public dialogRef: MatDialogRef<ContentDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: any) { }

  ngOnInit(): void {
    var reader = new FileReader();
      reader.readAsDataURL(this.data);
      reader.onload = (_event) => {
      this.url = reader.result;
      }
  }

  onNoClick(): void {
    this.dialogRef.close();
  }

  zoomPlus(): void{

    console.log(this.zoom)
    var value = parseInt(this.zoom.split("%")[0])
    
    console.log(value)
    value += 10
    this.zoom = value + "%"

    console.log(this.zoom)
  }

  zoomMinus(): void{

    console.log(this.zoom)
    var value = parseInt(this.zoom.split("%")[0])
    
    console.log(value)
    value -= 10
    this.zoom = value + "%"

    console.log(this.zoom)
  }
  
}
