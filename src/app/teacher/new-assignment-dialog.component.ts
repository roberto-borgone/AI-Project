import { Component, EventEmitter, Inject, OnInit, Output, ViewChild } from '@angular/core';
import { FormControl } from '@angular/forms';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';

export interface DialogData {
  dueDate: FormControl;
  file: any;
  error: string;
}

@Component({
  selector: 'app-new-assignment-dialog',
  templateUrl: './new-assignment-dialog.component.html',
  styleUrls: ['./new-assignment-dialog.component.css']
})
export class NewAssignmentDialogComponent implements OnInit {

  testo = "Carica consegna"
  isValid: boolean;
  isDisabled: boolean;

  constructor(public dialogRef: MatDialogRef<NewAssignmentDialogComponent>, 
    @Inject(MAT_DIALOG_DATA) public data: DialogData) {
      this.isValid = true;
      this.isDisabled = true;
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
    this.testo = "Consegna caricata"
    this.isValid = false
    this.isDisabled = false
  }
}
