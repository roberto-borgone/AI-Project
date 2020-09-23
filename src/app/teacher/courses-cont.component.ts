import { Component, OnDestroy } from '@angular/core';
import { Course } from '../models/course.model';
import { Subscription } from 'rxjs';
import { CourseService } from '../services/course.service';
import { Router, ActivatedRoute } from '@angular/router';
import { MatDialog } from '@angular/material/dialog';
import { FormControl, Validators } from '@angular/forms';
import { AddCourseDialogComponent } from './add-course-dialog.component';
import { ModifyCourseDialogComponent } from './modify-course-dialog.component';
import { AuthService } from '../auth/auth.service';
import { Teacher } from '../models/teacher.model';

@Component({
  selector: 'app-courses-cont',
  templateUrl: './courses-cont.component.html',
  styleUrls: ['./courses-cont.component.css']
})
export class CoursesContComponent implements OnDestroy {

  subscriptions: Subscription = new Subscription()
  courses: Course[]

  courseName: FormControl = new FormControl('', [Validators.required])
  courseAcronimo: FormControl = new FormControl('', [Validators.required])
  courseMin: FormControl = new FormControl('', [Validators.min(1)])
  courseMax: FormControl = new FormControl('', [Validators.min(1)])
  courseEnabled: FormControl = new FormControl(true)
  courseNameVM: FormControl = new FormControl('', [Validators.required])
  courseVersionVM: FormControl = new FormControl('', [Validators.required])
  owners: Teacher[] = []
  addCourseInvalid: boolean = false

  mcourseAcronimo: FormControl = new FormControl('', [Validators.required])
  mcourseMin: FormControl = new FormControl('', [Validators.min(1)])
  mcourseMax: FormControl = new FormControl('', [Validators.min(1)])

  interval: NodeJS.Timeout

  constructor(private courseService: CourseService, private router: Router, private route: ActivatedRoute, private dialog: MatDialog, private auth: AuthService) { 
    this.getCourses()
    this.interval = setInterval(() => {
      this.getCourses()
    }, 4000)
  }

  getCourses(){
    this.subscriptions.add(this.courseService.query(this.auth.token.role, this.auth.token.username).subscribe(courses => this.courses = courses))
  }

  toggleCourse(course: Course){
    this.subscriptions.add(this.courseService.updateStatus(course).subscribe(course => this.getCourses()))
  }

  deleteCourse(course: Course){
    this.subscriptions.add(this.courseService.deleteCourse(course).subscribe(course => {
      this.getCourses();
      this.router.navigate(['/teacher']);
    }))
  }

  openDialogAddCourse(): void {
    let dialogRef = this.dialog.open(AddCourseDialogComponent, {
      width: '400px',
      data: {courseName: this.courseName, courseAcronimo: this.courseAcronimo, courseMin: this.courseMin, courseMax: this.courseMax, courseEnabled: this.courseEnabled, courseNameVM: this.courseNameVM, courseVersionVM: this.courseVersionVM, addCourseInvalid: this.addCourseInvalid, owners: this.owners}
    });

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      if(result && result.courseName.valid && result.courseAcronimo.valid && result.courseMin.valid && result.courseMax.valid && result.courseNameVM.valid && result.courseVersionVM.valid && result.courseEnabled.valid){

        // nested observables.. i could have found a more elegant solution to this
        this.subscriptions.add(this.courseService.create(new Course(result.courseName.value, result.courseAcronimo.value, result.courseMin.value, result.courseMax.value, result.courseEnabled.value), result.owners)
        .subscribe(authResult => {
            
          if(authResult === false){
            // not logged error message display
            this.addCourseInvalid = true
            this.openDialogAddCourse()
          }else{

            this.subscriptions.add(this.courseService.createModelVM(result.courseName.value, result.courseNameVM.value, result.courseVersionVM.value).subscribe())
            // created
            this.addCourseInvalid = false
            this.courseName.reset()
            this.courseAcronimo.reset()
            this.courseMin.reset()
            this.courseMax.reset()
            this.courseEnabled.reset()
            this.owners = []
            this.getCourses()
          }
        }))
      }else if(!result){
        // dialog closed i remove data
        this.addCourseInvalid = false
        this.courseName.reset()
        this.courseAcronimo.reset()
        this.courseMin.reset()
        this.courseMax.reset()
        this.courseEnabled.reset()
        this.owners = []
      }
    }));
  }

  openDialogModifyCourse(course: Course): void {
    let dialogRef = this.dialog.open(ModifyCourseDialogComponent, {
      width: '400px',
      data: {mcourseAcronimo: this.mcourseAcronimo, mcourseMin: this.mcourseMin, mcourseMax: this.mcourseMax, course: course}
    });

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      if(result && result.mcourseAcronimo.valid && result.mcourseMin.valid && result.mcourseMax.valid){

        // nested observables.. i could have found a more elegant solution to this
        this.subscriptions.add(this.courseService.update(new Course(course.name, result.mcourseAcronimo.value, result.mcourseMin.value, result.mcourseMax.value, course.enabled))
        .subscribe(result => {
          this.getCourses()
          this.mcourseAcronimo.reset()
          this.mcourseMin.reset()
          this.mcourseMax.reset()
        }))
      }else if(!result){
        this.mcourseAcronimo.reset()
        this.mcourseMin.reset()
        this.mcourseMax.reset()
      }
    }));
  }


  ngOnDestroy(): void {
    clearInterval(this.interval)
    this.subscriptions.unsubscribe()
  }

}
