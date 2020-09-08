import { Injectable } from '@angular/core'
import { Student} from 'src/app/student.model'
import { Observable, throwError, from } from 'rxjs'
import { HttpClient, HttpHeaders } from '@angular/common/http'
import { catchError, concatMap, toArray, map } from 'rxjs/operators';
import { CourseService } from './course.service';

export interface GroupEntity{
  id: number
  name: string
}

export interface StudentEntity{
  id: string
  name: string
  surname: string
  email: string
  courseId: string
  groupId: number
  group: GroupEntity
}

@Injectable({
  providedIn: 'root'
})
export class StudentService {

  private API_PATH = 'https://localhost:4200/api/API/students'

  httpOptions = {
    headers: new HttpHeaders({
      'Content-Type': 'application/json'
    })
  }
                   
  constructor(private http: HttpClient, private courseService: CourseService) {}

  create(student: Student){
    this.http.post<Student>(this.API_PATH, student, this.httpOptions)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  update(student: Student){
    this.http.put<Student>(this.API_PATH, student, this.httpOptions)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  find(id: number): Observable<Student>{
    return this.http.get<Student>(this.API_PATH + '/' + id)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  query(): Observable<Student[]>{
    //return of(this.students)
    return this.http.get<Student[]>(this.API_PATH)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  delete(id: number){
    this.http.delete(this.API_PATH + '/' + id)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  enroll(courseId: string, student: Student): Observable<boolean>{
    return this.http.post<Object>(this.courseService.API_PATH + '/' + this.courseService.currentCourse.name + '/enrollOne', {id: student.id}, this.httpOptions)
    .pipe(
      map(result => {return true})
    )
  }

  deleteFromCourse(courseId: string, students: Student[]): Observable<Student[]>{
    return from(students).pipe(
      concatMap(student => {
        student.courseId = courseId
        return this.http.post<Student>(this.courseService.API_PATH + '/' + this.courseService.currentCourse.name + '/deleteStudentFromCourse/' + student.id, student, this.httpOptions)
      }),
      toArray()
    )
  }

  getEnrolled(courseId: string): Observable<Student[]>{

    let endpoint: string = 'https://localhost:4200/api/API/courses' + '/' + courseId + '/' + 'enrolled'

    return this.http.get<StudentEntity[]>(endpoint)
    .pipe(
      map(students => { 
        let studentsDTO: Student[] = []
        students.forEach(student => studentsDTO.push(new Student(student.id, 
                                                                 student.name, 
                                                                 student.surname, 
                                                                 student.courseId, 
                                                                 student.group?student.group.name:'<none>')))
        return studentsDTO
      }),
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }
}
