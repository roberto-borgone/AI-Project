import { Component, Inject } from '@angular/core';
import {MatDialogRef, MAT_DIALOG_DATA} from '@angular/material/dialog';
import {FormControl} from '@angular/forms';
import { Router } from '@angular/router';

export interface DialogData {
  username: FormControl;
  password: FormControl;
  loginInvalid: Boolean
  message: string
}

@Component({
  selector: 'app-login-dialog',
  templateUrl: './login-dialog.component.html',
  styleUrls: ['./login-dialog.component.css']
})
export class LoginDialogComponent {

  constructor(private router: Router,
    public dialogRef: MatDialogRef<LoginDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData) {}

  onNoClick(): void {
    this.dialogRef.close();
  }

  getErrorMessageUsername(username: FormControl) {
    if (username.hasError('required')) {
      return 'Inserisci la tua matricola';
    }
  }

  getErrorMessagePassword(password: FormControl) {
    if (password.hasError('required')) {
      return 'Inserisci una password';
    }

    return password.hasError('minlength') ? 'La password deve contenere almeno 6 caratteri' : '';
  }

}
