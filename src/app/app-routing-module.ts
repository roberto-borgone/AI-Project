import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { HomeComponent } from './home.component';
import { VmsContComponent } from './teacher/vms-cont.component';
import { PageNotFoundComponent } from './page-not-found.component';
import { StudentsContComponent } from './teacher/students-cont.component';
import { AuthGuard } from './auth/auth.guard';
import { HomeSidevanComponent } from './home-sidevan.component';
import { CoursesContComponent } from './teacher/courses-cont.component';
import { ProfileComponent } from './profile.component';
import { StudentCourseComponent } from './student/student-course.component';

const routes: Routes = [
    { 
        path: 'home', 
        children: [
            { path: '', component: HomeSidevanComponent, outlet: 'sidenav'},
            { path: '', component: HomeComponent}
        ]
    },
    { path: '', redirectTo:'/home', pathMatch: 'full' },
    { 
        path: 'teacher',
        canActivate: [AuthGuard],
        children: [
            { path: '', component: HomeComponent },
            { path: ':id/students', component: StudentsContComponent },
            { path: ':id/vms', component: VmsContComponent },
            { path: '', component: CoursesContComponent, outlet: 'sidenav'}
        ]
    },
    { 
        path: 'student',
        canActivate: [AuthGuard],
        children: [
            { path: '', component: HomeComponent },
            { path: ':id/students', component: StudentsContComponent },
            { path: ':id/vms', component: VmsContComponent },
            { path: '', component: StudentCourseComponent, outlet: 'sidenav'}
        ]
    },
    { path: 'profile', component: ProfileComponent},
    { path: '**', component: PageNotFoundComponent },
];

@NgModule({
    imports: [RouterModule.forRoot(routes, {enableTracing: false})],
    exports: [RouterModule]
})

export class AppRoutingModule { }