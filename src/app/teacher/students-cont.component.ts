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
    this.subscriptions.add(this.router.events.subscribe(val => {
      if(val instanceof NavigationEnd){
        this.getStudents()
        this.getEnrolledStudents()
      }
    }))
  }

  deleteStudent(students: Student[]){
    this.subscriptions.add(this.studentService.deleteFromCourse(students).subscribe(x => {this.getStudents(); this.getEnrolledStudents()}))
  }

  addStudent(student: Student){
    this.subscriptions.add(this.studentService.enroll(student).subscribe(x => {this.getStudents(); this.getEnrolledStudents()}))
  }

  getStudents(){
    this.subscriptions.add(this.studentService.query().subscribe(students => this.students = students))
  }

  getEnrolledStudents(){
    this.subscriptions.add(this.studentService.getEnrolled(this.courseService.currentCourse.name).subscribe(students => this.enrolledStudents = students))
  }

  uploadFile(file: any) {
    console.log("Sono in uploadFile in students-cont");
    this.subscriptions.add(this.courseService.uploadFile(file).subscribe(uploadResult => {
      this.csvResult = uploadResult;
      console.log(this.csvResult);
      this.openDialog();
    }));
  }

  openDialog() {
    const dialogConfig = new MatDialogConfig();

    dialogConfig.autoFocus = true;

    dialogConfig.data = this.csvResult;
    dialogConfig.width = '800px';

    const dialogRef = this.dialog.open(CsvUploadDialogComponent, dialogConfig);

    //this.subscriptions.add(dialogRef.afterClosed().subscribe());
}

  ngOnDestroy(){
    this.subscriptions.unsubscribe()
  }
}
