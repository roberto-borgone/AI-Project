import { Component, EventEmitter, Inject, OnInit, Output } from '@angular/core';
import { FormControl } from '@angular/forms';
import { MatDatepicker, MatDatepickerInput, MatDatepickerInputEvent } from '@angular/material/datepicker';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';

export interface DialogData {
  dueDate: FormControl;
  file: any;
}

@Component({
  selector: 'app-new-assignment-dialog',
  templateUrl: './new-assignment-dialog.component.html',
  styleUrls: ['./new-assignment-dialog.component.css']
})
export class NewAssignmentDialogComponent implements OnInit {


  constructor(public dialogRef: MatDialogRef<NewAssignmentDialogComponent>, 
    @Inject(MAT_DIALOG_DATA) public data: DialogData) {
   }

  ngOnInit(): void {
  }

  onNoClick(): void {
    this.dialogRef.close();
  }

  onClick() { 
    const fileUpload = document.getElementById('imageUpload') as HTMLInputElement;
    fileUpload.click();
  }

  handleImageSelect(event: any) {
    console.log("Sono in handleImageSelect");
    var files = event.target.files; // FileList object
    var file = files[0];
    this.data.file = file;
  }
}
