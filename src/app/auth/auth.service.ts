import { Injectable } from '@angular/core';
import { User } from '../user.model';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { catchError, map } from 'rxjs/operators';
import { Observable, of } from 'rxjs';
import * as moment from 'moment';
import { Router } from '@angular/router';
import { RegistartionForm } from '../registrationForm.model';

export interface Token {
  //accessToken is for the mock server
  accessToken: string;
  token: string;
  username: string;
}

export interface TokenPayload {
  email: string
  iat: number
  exp: number
  sub: string    
}

@Injectable({
  providedIn: 'root'
})
export class AuthService {

  API_PATH = 'https://localhost:4200/api/auth/signin'
  API_PATH_REGISTER = 'https://localhost:4200/api/auth/register'

  token: Token
  redirectUrl: string

  constructor(private http: HttpClient, private router: Router) {}

  httpOptions = {
    headers: new HttpHeaders({
      'Content-Type': 'application/json'
    })
  }

  login(user: User): Observable<Boolean>{
    // i remove an eventual stored token, otherwise the interceptor fails
    this.token = undefined
    // login
    return this.http.post<Token>(this.API_PATH, user, this.httpOptions)
    .pipe(
      map(token => {
        this.token = token
        if(this.redirectUrl)
          this.router.navigate([this.redirectUrl])
        return true}),
      catchError( err => {
        return of(false)
      })
    )
  }

  logout(){
    // remove stored url and token
    this.redirectUrl = undefined
    this.token = undefined
  }

  isAuth(): boolean{
    // if there is no token or it is expired i am not logged
    if(!this.token)
      return false

    let payload: TokenPayload = JSON.parse(atob(this.token.token.split('.')[1]))
    return moment().isBefore(moment.unix(payload.exp))
  }

  jwtExpired(): boolean{
    // the token is expired if is present and expired

    if(!this.isAuth() && (this.token?true:false)){
      // if expired i remove it (probably redundant, when jwt expires and i am redirected to login i already remove the token before create a new one)
      this.logout()
      return true
    }
    return false 
  }

  register(registrationForm: RegistartionForm): Observable<Boolean>{
    return this.http.post(this.API_PATH_REGISTER, registrationForm, this.httpOptions)
    .pipe(
      map(result => {
        return true}),
      catchError( err => {
        console.log(err)
        return of(false)
      })
    )
  }

}
