import { Component, OnDestroy } from '@angular/core';
import { Course } from '../course.model';
import { Subscription } from 'rxjs';
import { CourseService } from '../services/course.service';

@Component({
  selector: 'app-courses-cont',
  templateUrl: './courses-cont.component.html',
  styleUrls: ['./courses-cont.component.css']
})
export class CoursesContComponent implements OnDestroy {

  subscriptions: Subscription = new Subscription()
  courses: Course[]

  constructor(private courseService: CourseService) { 
    this.getCourses()
  }

  getCourses(){
    this.subscriptions.add(this.courseService.query().subscribe(courses => this.courses = courses))
  }

  ngOnDestroy(): void {
    this.subscriptions.unsubscribe()
  }

}
