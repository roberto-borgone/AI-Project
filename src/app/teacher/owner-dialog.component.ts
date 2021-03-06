import { Component, OnInit, Inject } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { Student } from '../models/student.model';

export interface DialogData{
  students: Student[]
}

@Component({
  selector: 'app-owner-dialog',
  templateUrl: './owner-dialog.component.html',
  styleUrls: ['./owner-dialog.component.css']
})
export class OwnerDialogComponent implements OnInit {

  constructor( public dialogRef: MatDialogRef<OwnerDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData) { }

  ngOnInit(): void {
  }

  onNoClick(): void {
    this.dialogRef.close();
  }

}
