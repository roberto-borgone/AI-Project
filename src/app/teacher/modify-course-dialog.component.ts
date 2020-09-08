import { Component, Inject } from '@angular/core';
import { Router } from '@angular/router';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { FormControl } from '@angular/forms';

export interface DialogData {
  mcourseAcronimo: FormControl;
  mcourseMin: FormControl;
  mcourseMax: FormControl; 
}

@Component({
  selector: 'app-modify-course-dialog',
  templateUrl: './modify-course-dialog.component.html',
  styleUrls: ['./modify-course-dialog.component.css']
})
export class ModifyCourseDialogComponent {

  constructor(private router: Router,
    public dialogRef: MatDialogRef<ModifyCourseDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData) {}

  onNoClick(): void {
    this.dialogRef.close();
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
