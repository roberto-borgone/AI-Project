import { Injectable } from '@angular/core';
import { CanActivate, ActivatedRouteSnapshot, RouterStateSnapshot, UrlTree, Router, NavigationExtras } from '@angular/router';
import { Observable } from 'rxjs';
import { AuthService } from './auth.service';

@Injectable({
  providedIn: 'root'
})
export class AuthGuard implements CanActivate {

  constructor(private auth: AuthService, private router: Router){}

  canActivate(
    next: ActivatedRouteSnapshot,
    state: RouterStateSnapshot): Observable<boolean | UrlTree> | Promise<boolean | UrlTree> | boolean | UrlTree {
    return this.checkLogin(state.url) && this.checkRole(state.url);
  }

  checkLogin(url: string): boolean {
    // if i'm logged i can visit the protected url
    if (this.auth.isAuth()) { return true; }

    // otherwise

    // store the attempted URL for redirecting
    this.auth.redirectUrl = url;
    let navigationExtras: NavigationExtras = {
      queryParams: {'doLogin': true}
    }
    
    // navigate to the login page with extras
    this.router.navigate(['/home'], navigationExtras);
    return false;
    }

    checkRole(url: string): boolean{
      if((url.includes('teacher') && this.auth.token.role == 'teacher') || (url.includes('student') && !url.includes('students') && this.auth.token.role == 'student'))
        return true
      else{
        this.router.navigate(['/' + this.auth.token.role]);
        return false
      }
    }
  
}
