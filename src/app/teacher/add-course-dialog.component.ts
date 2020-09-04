import { Component, Inject } from '@angular/core';
import { FormControl } from '@angular/forms';
import { Router } from '@angular/router';
import { MAT_DIALOG_DATA, MatDialogRef } from '@angular/material/dialog';

export interface DialogData {
  courseName: FormControl;
  courseAcronimo: FormControl;
  courseMin: FormControl;
  courseMax: FormControl;
  courseEnabled: FormControl;
  addCourseInvalid: boolean; 
}

@Component({
  selector: 'app-add-course-dialog',
  templateUrl: './add-course-dialog.component.html',
  styleUrls: ['./add-course-dialog.component.css']
})
export class AddCourseDialogComponent {

  constructor(private router: Router,
    public dialogRef: MatDialogRef<AddCourseDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData) {}

  onNoClick(): void {
    this.dialogRef.close();
  }

  getErrorMessageName(courseName: FormControl) {
    if (courseName.hasError('required')) {
      return 'Inserisci un nome';
    }
  }

  getErrorMessageAcronimo(courseAcronimo: FormControl) {
    if (courseAcronimo.hasError('required')) {
      return 'Inserisci un acronimo';
    }
  }

  getErrorMessageMin(courseMin: FormControl) {
    if (courseMin.hasError('min')) {
      return 'Inserisci un numero valido di studenti';
    }
  }

  getErrorMessageMax(courseMax: FormControl) {
    if (courseMax.hasError('min')) {
      return 'Inserisci un numero valido di studenti';
    }
  }

}
