import { Component, Inject } from '@angular/core';
import { FormControl } from '@angular/forms';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { Router } from '@angular/router';
import { AuthService } from '../auth/auth.service';
import { Student } from '../student.model';

export interface DialogData {
  students: Student[]
  newTeamName: FormControl;
  newTeamInvalid: boolean;
  message: string
}

@Component({
  selector: 'app-new-team-dialog',
  templateUrl: './new-team-dialog.component.html',
  styleUrls: ['./new-team-dialog.component.css']
})
export class NewTeamDialogComponent {

  message: string

  constructor(public auth: AuthService, private router: Router,
    public dialogRef: MatDialogRef<NewTeamDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData) {
      this.message = data.message
    }

  onNoClick(): void {
    this.dialogRef.close();
  }

  getErrorMessageName(newTeamName: FormControl) {
    if (newTeamName.hasError('required')) {
      return 'Inserisci un nome';
    }
  }

  getMessage(){
    return this.message;
  }

}
