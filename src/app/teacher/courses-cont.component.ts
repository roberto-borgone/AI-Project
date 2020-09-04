import { Component, OnDestroy } from '@angular/core';
import { Course } from '../course.model';
import { Subscription } from 'rxjs';
import { CourseService } from '../services/course.service';
import { NavigationEnd, Router } from '@angular/router';

@Component({
  selector: 'app-courses-cont',
  templateUrl: './courses-cont.component.html',
  styleUrls: ['./courses-cont.component.css']
})
export class CoursesContComponent implements OnDestroy {

  subscriptions: Subscription = new Subscription()
  courses: Course[]

  constructor(private courseService: CourseService, private router: Router) { 
    this.getCourses()
    this.subscriptions.add(this.router.events.subscribe(val => {
      if(val instanceof NavigationEnd){
        this.getCourses()
      }
    }))
  }

  getCourses(){
    this.subscriptions.add(this.courseService.query().subscribe(courses => this.courses = courses))
  }

  toggleCourse(course: Course){
    this.subscriptions.add(this.courseService.updateStatus(course).subscribe(course => this.getCourses()))
  }

  ngOnDestroy(): void {
    this.subscriptions.unsubscribe()
  }

}
