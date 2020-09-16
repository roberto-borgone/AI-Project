import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable, throwError } from 'rxjs';
import { VM } from '../vm.model';
import { catchError, map } from 'rxjs/operators';
import { CourseService } from './course.service';
import { AuthService } from '../auth/auth.service';
import { Student } from '../student.model';

@Injectable({
  providedIn: 'root'
})
export class VmService {

  httpOptions = {
    headers: new HttpHeaders({
      'Content-Type': 'application/json'
    })
  }

  API_PATH: string = 'https://localhost:4200/api/API/vms'

  constructor(private http: HttpClient, private courseService: CourseService, private authService: AuthService) { }

  query(): Observable<VM[]>{

    let PATH = 'https://localhost:4200/api/API/' + this.authService.token.role == 'teacher'?'docents':

    return this.http.get<VM[]>(PATH + '/' + this.authService.token.username + '/' + this.courseService.currentCourse.name + '/getVMS', this.httpOptions)
    .pipe(
      map(result => {
        for(let vm of result){
          this.http.get<Student[]>(this.API_PATH + '/' + vm.id + '/owners').subscribe(owners => {vm.owners = owners})
        }
        return result
      }),
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }
}
