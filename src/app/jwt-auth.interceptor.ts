import { Injectable } from '@angular/core';
import {
  HttpRequest,
  HttpHandler,
  HttpEvent,
  HttpInterceptor
} from '@angular/common/http';
import { Observable, throwError } from 'rxjs';
import { AuthService } from './auth/auth.service';
import { Router, NavigationExtras } from '@angular/router';

@Injectable()
export class JwtAuthInterceptor implements HttpInterceptor {

  constructor(private auth: AuthService, private router: Router) {}

  intercept(request: HttpRequest<any>, next: HttpHandler): Observable<HttpEvent<any>> {
    if(this.auth.isAuth()){
      // if i'm authenticated i set the auth header
      const authToken = this.auth.token.token
      const authReq = request.clone({
        headers: request.headers.set('Authorization', 'Bearer ' + authToken)
        });
      return next.handle(authReq);
    
    }else if(this.auth.jwtExpired()){
      // if i'm not authenticated and i do a request to the server i store the page and i redirect to the login (all the requests i do to the server involve protected resources)
      this.auth.redirectUrl = this.router.url
      let navigationExtras: NavigationExtras = {
        queryParams: {'doLogin': true}
      }
      this.router.navigate(['/home'], navigationExtras);
      // i stop the interceptor chain
      return throwError('Unauthorized request redirecting to login')
    
    }else{
      // if i don't have any jwt it means i am not logged (not by jwt expiration) i forward the request
      return next.handle(request)
    }
  }
}
