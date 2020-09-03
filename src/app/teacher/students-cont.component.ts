import { Component, OnDestroy } from '@angular/core';
import { Student} from 'src/app/student.model'
import { StudentService } from '../services/student.service';
import { Subscription, Observable } from 'rxjs';
import { CourseService } from '../services/course.service';
import { Router, NavigationEnd } from '@angular/router';
import { MatDialog, MatDialogConfig } from '@angular/material/dialog';
import { CsvUploadDialogComponent } from './csv-upload-dialog.component';
import { CsvStudent } from '../csv-student.model';

@Component({
  selector: 'app-students-cont',
  templateUrl: './students-cont.component.html',
  styleUrls: ['./students-cont.component.css']
})
export class StudentsContComponent implements OnDestroy{

  enrolledStudents: Student[]
  students: Student[]
  subscriptions: Subscription = new Subscription()
  csvResult : CsvStudent[]

  constructor(private studentService: StudentService, private courseService: CourseService, private router: Router, public dialog: MatDialog) {
    this.getStudents()
    this.getEnrolledStudents()
    this.subscriptions.add(this.router.events.subscribe(val => {
      if(val instanceof NavigationEnd){
        this.getStudents()
        this.getEnrolledStudents()
      }
    }))
  }

  deleteStudent(students: Student[]){
    this.subscriptions.add(this.studentService.updateEnrolled('', students).subscribe(x => this.getEnrolledStudents()))
  }

  addStudent(students: Student[]){
    this.subscriptions.add(this.studentService.updateEnrolled(this.courseService.currentCourse.name, students).subscribe(x => this.getEnrolledStudents()))
  }

  getStudents(){
    this.subscriptions.add(this.studentService.query().subscribe(students => this.students = students))
  }

  getEnrolledStudents(){
    this.subscriptions.add(this.studentService.getEnrolled(this.courseService.currentCourse.name).subscribe(students => this.enrolledStudents = students))
  }

  uploadFile(file: any) {
    console.log("Sono in uploadFile in students-cont");
    this.subscriptions.add(this.studentService.uploadFile(file).subscribe(uploadResult => {
      console.log(uploadResult);
      this.csvResult = uploadResult;
      this.openDialog();
    }));
  }

  openDialog() {
    const dialogConfig = new MatDialogConfig();

    dialogConfig.disableClose = true;
    dialogConfig.autoFocus = true;

    dialogConfig.data = this.csvResult;

    this.dialog.open(CsvUploadDialogComponent, dialogConfig);
}

  ngOnDestroy(){
    this.subscriptions.unsubscribe()
  }
}
