import { Component, Inject } from '@angular/core';
import {MatDialogRef, MAT_DIALOG_DATA} from '@angular/material/dialog';
import {FormGroup} from '@angular/forms';
import { MyErrorStateMatcher } from '../app.component';

export interface DialogData {
  matcher: MyErrorStateMatcher
  formGroup: FormGroup
  registrationInvalid: Boolean
}

@Component({
  selector: 'app-register',
  templateUrl: './register.component.html',
  styleUrls: ['./register.component.css']
})
export class RegisterComponent {

  constructor(
    public dialogRef: MatDialogRef<RegisterComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData) {}

  onNoClick(): void {
    this.dialogRef.close();
  }

  getErrorMessageEmail(formGroup: FormGroup) {
    if (formGroup.controls.email.hasError('required')) {
      return 'Inserisci una email';
    }

    if(formGroup.controls.email.hasError('email')){
      return 'Email non valida'
    }

    return "Inserisci la tua mail istituzionale (ex. s267571@studenti.polito.it)"
  }

  getErrorMessagePassword(formGroup: FormGroup) {
    if (formGroup.controls.password.hasError('required')) {
      return 'Inserisci una password';
    }

    return formGroup.controls.password.hasError('minlength') ? 'La password deve contenere almeno 6 caratteri' : '';
  }

  getErrorMessageName(formGroup: FormGroup) {
    if (formGroup.controls.name.hasError('required')) {
      return 'Inserisci il tuo nome';
    }
  }

  getErrorMessageSurname(formGroup: FormGroup) {
    if (formGroup.controls.surname.hasError('required')) {
      return 'Inserisci il tuo cognome';
    }
  }

  getErrorMessageId(formGroup: FormGroup) {
    if (formGroup.controls.id.hasError('required')) {
      return 'Inserisci la tua matricola';
    }

    return 'Inserisci una matricola valida (ex. s267571)'
  }

}
