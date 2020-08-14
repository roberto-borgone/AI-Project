import { Component, ViewChild, OnDestroy } from '@angular/core';
import { MatSidenav } from '@angular/material/sidenav';
import {MatDialog} from '@angular/material/dialog';
import { LoginDialogComponent } from './auth/login-dialog.component';
import { RegisterComponent } from './auth/register.component'
import { SuccessRegisterDialogComponent } from './auth/success-register-dialog.component'
import {FormControl, Validators, FormGroupDirective, NgForm, FormGroup} from '@angular/forms';
import { AuthService } from './auth/auth.service';
import { User } from './user.model';
import { ActivatedRoute, Router } from '@angular/router';
import { Subscription } from 'rxjs';
import { RegistartionForm } from './registrationForm.model';
import { ErrorStateMatcher } from '@angular/material/core';
import { CourseService } from './services/course.service';

export class MyErrorStateMatcher implements ErrorStateMatcher {
  isErrorState(control: FormControl | null, form: FormGroupDirective | NgForm | null): boolean {
    return !!(control && control.touched && control.parent && control.parent.hasError('notSame'));
  }
}

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnDestroy{
  
  title = 'ai20-lab04';

  subscriptions: Subscription = new Subscription()

  @ViewChild('sidenav')
  sidenav: MatSidenav

  // sidenav toggle
  toggleForMenuClick() {
    this.sidenav.toggle()
  }

  username: FormControl = new FormControl('', [Validators.required])
  password: FormControl = new FormControl('', [Validators.required, Validators.minLength(6)])
  loginInvalid: Boolean = false
  logged: boolean = false

  matcher = new MyErrorStateMatcher()
  formGroup: FormGroup = new FormGroup({
    email:  new FormControl('', [Validators.required, Validators.email, Validators.pattern(/^(s|d)[0-9]{6}@(polito\.it|studenti\.polito\.it)$/)]),
    password: new FormControl('', [Validators.required, Validators.minLength(6)]),
    passwordConfirm: new FormControl(''),
    name: new FormControl('', [Validators.required]),
    surname: new FormControl('', [Validators.required]),
    id: new FormControl('', [Validators.required, Validators.pattern(/^(s|d)[0-9]{6}$/)])
  }, this.checkPasswords)
  registrationInvalid: Boolean = false


  constructor(public dialog: MatDialog, private auth: AuthService, private route: ActivatedRoute, private router: Router, public courseService: CourseService) {
    this.subscriptions.add(this.route.queryParams.subscribe(params => {
      if(params['doLogin'] === 'true'){
        this.logged = false
        this.openDialog()
      }
      if(params['doRegister'] === 'true'){
        this.openDialogRegistration()
      }
    }))
  }

  openDialog(): void {
    let dialogRef = this.dialog.open(LoginDialogComponent, {
      width: '400px',
      data: {username: this.username, password: this.password, loginInvalid: this.loginInvalid}
    });

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      if(result && result.username.valid && result.password.valid){

        // if i do not remove doLogin from url and the jwt expires i'm not able to login anymore
        if(!this.auth.redirectUrl){
          this.auth.redirectUrl= '/teacher'
        }
        // nested observables.. i could have found a more elegant solution to this
        this.subscriptions.add(this.auth.login(new User(result.username.value, result.password.value))
        .subscribe(authResult => {
            
          if(authResult === false){
            // not logged error message display
            this.loginInvalid = true
            this.openDialog()
          }else{
            // logged i remove credentials from dialog to hide them at the next login
            this.loginInvalid = false
            this.username.reset()
            this.password.reset()
            this.logged = true
            this.auth.redirectUrl = undefined
          }
        }))
      }else if(!result){
        // dialog closed i remove the credentials
        this.username.reset()
        this.password.reset()
        this.loginInvalid = false
        // i remove an eventual stored url (the user dicided to not login and restore the old url)
        this.auth.redirectUrl = undefined
        // if i do not remove doLogin from url i press cancel i'm not able to login anymore
        this.router.navigate(['/home'])
      }
    }));
  }

  openDialogRegistration(): void {
    let dialogRef = this.dialog.open(RegisterComponent, {
      width: '400px',
      data: {matcher: this.matcher, formGroup: this.formGroup, registrationInvalid: this.registrationInvalid}
    });

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      if(result && result.formGroup.controls.email.valid 
        && result.formGroup.controls.password.valid 
        && result.formGroup.controls.passwordConfirm.valid
        && result.formGroup.controls.name.valid 
        && result.formGroup.controls.surname.valid 
        && result.formGroup.controls.id.valid
        && !result.formGroup.hasError('notSame')){

        this.subscriptions.add(this.auth.register(new RegistartionForm(result.formGroup.controls.email.value, 
                                                                       result.formGroup.controls.password.value, 
                                                                       result.formGroup.controls.passwordConfirm.value,
                                                                       result.formGroup.controls.name.value,
                                                                       result.formGroup.controls.surname.value,
                                                                       result.formGroup.controls.id.value
                                                                       ))
        .subscribe(registrationResult => {
            
          if(registrationResult === false){
            this.registrationInvalid = true
            this.openDialogRegistration()
          }else{
            this.formGroup.controls.email.reset()
            this.formGroup.controls.password.reset()
            this.formGroup.controls.passwordConfirm.reset()
            this.formGroup.controls.name.reset()
            this.formGroup.controls.surname.reset()
            this.formGroup.controls.id.reset()
            this.registrationInvalid = false
            this.auth.redirectUrl = undefined
            this.router.navigate(['/home'])
            this.openSuccessRegistrationDialog()
          }
        }))
      }else if(!result){

        // dialog closed i remove the credentials
        this.formGroup.controls.email.reset()
        this.formGroup.controls.password.reset()
        this.formGroup.controls.passwordConfirm.reset()
        this.formGroup.controls.name.reset()
        this.formGroup.controls.surname.reset()
        this.formGroup.controls.id.reset()
        this.registrationInvalid = false
        this.router.navigate(['/home'])
      }
    }));
  }

  openSuccessRegistrationDialog(): void{
    let dialogRef = this.dialog.open(SuccessRegisterDialogComponent, {
      width: '400px'
    });
  }

  logout(){
    // remove credentials (probably reduntant)
    this.logged = false
    this.username.reset()
    this.password.reset()
    this.loginInvalid = false
    //remove token and an eventual stored url (the user decided to logout, the application is reset)
    this.auth.logout() 
  }

  ngOnDestroy(){
    this.subscriptions.unsubscribe()
  }

  checkPasswords(formGroup: FormGroup) { // here we have the 'passwords' group
    let pass = formGroup.controls.password.value;
    let confirmPass = formGroup.controls.passwordConfirm.value;

    return pass === confirmPass ? null : { notSame: true }
  }

  eraseCurrentCourse(){
    this.courseService.currentCourse = undefined
  }

}
