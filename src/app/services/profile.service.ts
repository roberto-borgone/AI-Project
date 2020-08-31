import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { catchError } from 'rxjs/operators';
import { throwError, Observable } from 'rxjs';
import { AuthService } from '../auth/auth.service';
import { Profile } from '../profile.model';

@Injectable({
  providedIn: 'root'
})
export class ProfileService {

  constructor(private http: HttpClient, private authService : AuthService) { }

  sendImg(img : File){

    const API_PATH = 'https://localhost:4200/api/API/students/'+this.authService.token.username+'/addPhoto';

    const formData = new FormData();
    formData.append('imagefile', img);

    this.http.post(API_PATH, formData)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    ) 
  }

  getImg() : Observable<File> {

    console.log("Sono in getImg");

    const API_PATH = 'https://localhost:4200/api/API/students/'+this.authService.token.username+'/getPhoto';

    return this.http.get<File>(API_PATH)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    ) 
  }

  getProfile() : Observable<Profile> {
    const API_PATH = 'https://localhost:4200/api/API/students/'+this.authService.token.username;

    return this.http.get<Profile>(API_PATH)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }
}
