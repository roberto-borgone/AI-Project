import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { catchError, map } from 'rxjs/operators';
import { throwError, Observable, of } from 'rxjs';
import { AuthService } from '../auth/auth.service';
import { Profile } from '../models/profile.model';

@Injectable({
  providedIn: 'root'
})
export class ProfileService {

  constructor(private http: HttpClient, private authService : AuthService) { }

  sendImg(img : File): Observable<boolean>{


    const API_PATH = 'https://localhost:4200/api/API/students/'+this.authService.token.username+'/addPhoto';

    const formData = new FormData();
    formData.append('imagefile', img);

    return this.http.post(API_PATH, formData)
    .pipe(
      map(result => true),
      catchError( err => {
        console.error(err)
        return of(false)
      })
    ) 
  }

  getImg() : Observable<any> {


    const requestOptions: Object = {
      responseType: 'blob'
    }

    const API_PATH = 'https://localhost:4200/api/API/students/'+this.authService.token.username+'/getPhoto';

    return this.http.get<any>(API_PATH, requestOptions)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    ) 
  }

  getProfile() : Observable<Profile> {
    if(this.authService.token.role === "student"){
      var API_PATH = 'https://localhost:4200/api/API/students/'+this.authService.token.username;
    } else {
      var API_PATH = 'https://localhost:4200/api/API/docents/'+this.authService.token.username;
    }

    return this.http.get<Profile>(API_PATH)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }
}
