import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { from, Observable, of, throwError } from 'rxjs';
import { catchError, concatMap, map } from 'rxjs/operators';
import { CourseService } from './course.service';
import { AuthService } from '../auth/auth.service';
import { VM } from '../models/vm.model';
import { Student } from '../models/student.model';
import { TeamService } from './team.service';

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

  constructor(private http: HttpClient, private courseService: CourseService, private authService: AuthService, private teamService: TeamService) { }

  query(): Observable<VM[]>{

    let PATH = 'https://localhost:4200/api/API/docents'

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

  getVM(): Observable<VM[]>{

    let PATH = 'https://localhost:4200/api/API/students'

    return this.teamService.getGroup().pipe(
      concatMap(result => {
        if(result){
          return this.http.get<VM[]>(PATH + '/' + this.authService.token.username + '/teams/' + this.authService.token.group.id + '/getVMS', this.httpOptions).pipe(
            map(vms => {
              for(let vm of vms){
                vm.owners = []
                this.http.get<Student[]>(this.API_PATH + '/' + vm.id + '/owners').subscribe(owners => {vm.owners = owners})
              }
              return vms
            })
          )
        }else{
          let resultQuery: VM[] = []
          return of(resultQuery)
        }
      })
    )
  }

  newVM(ram: number, disk: number, vcpu: number): Observable<any>{

    let PATH = 'https://localhost:4200/api/API/teams'

    return this.teamService.getGroup().pipe(
      concatMap(result => {
        if(result){
          return this.http.post<Object>(PATH + '/' + this.authService.token.group.id + '/newVM/', {ram: ram, disk: disk, vcpu: vcpu}, this.httpOptions).pipe(
            map(result => {
              return of(result)
            })
          )
        }else{
          return of(new Error('Lo studente non fa parte di un team'))
        }
      }),
      catchError(err => {
        console.error(err)
        return of(err)
      })
    )

  }

  updateVM(vmID: number, ram: number, disk: number, vcpu: number): Observable<any>{
    return this.http.post<Object>(this.API_PATH + '/' + vmID + '/updateResurces/', {ram: ram, disk: disk, vcpu: vcpu}, this.httpOptions).pipe(
      map(result => {
        return of(result)
      }),
      catchError(err => {
        console.error(err)
        return of(err)
      })
    )

  }

  delete(vmID: number): Observable<boolean>{
    return this.http.delete(this.API_PATH + '/' + vmID + '/delete/', this.httpOptions).pipe(
      map(result => {
        return true
      }),
      catchError(err => {
        console.error(err)
        return of(false)
      })
    )
  }

  startVM(vmID: number): Observable<any> {
    console.log(vmID)

    return this.http.get(this.API_PATH + '/' +vmID + '/changeStatus/', this.httpOptions)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )

  }

  stopVM(vmID: number) : Observable<any>{
    console.log(vmID)

    return this.http.get(this.API_PATH + '/' + vmID + '/changeStatus/', this.httpOptions).pipe(
      map(result => {
        return true
      }),
      catchError(err => {
        console.error(err)
        return of(false)
      })
    )
  }

  makeOwners(owners: Student[], vm: VM): Observable<boolean>{
    return from(owners).pipe(
      concatMap(owner => {
        return this.http.post(this.API_PATH + '/' + vm.id + '/makeOwner', {id: owner.id}, this.httpOptions)
      }),
      map(result => {
        return true
      }),
      catchError(err => {
        console.error(err)
        return of(false)
      })
    )
  }

}
