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
  courseNameVM: FormControl;
  courseVersionVM: FormControl;
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

  sistemiOp: string[] = ['ios', 'ububtu', 'windows'];

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
    if (courseMin.hasError('required')) {
      return 'Inserisci un numero valido di studenti';
    }
  }

  getErrorMessageMax(courseMax: FormControl) {
    if (courseMax.hasError('required')) {
      return 'Inserisci un numero valido di studenti';
    }
  }

  getErrorMessageNameVM(courseNameVM: FormControl) {
    if (courseNameVM.hasError('required')) {
      return 'Inserisci un sistema operativo';
    }
  }

  getErrorMessageVersionVM(courseVersionVM: FormControl) {
    if (courseVersionVM.hasError('required')) {
      return 'Inserisci una versione';
    }
  }

}
