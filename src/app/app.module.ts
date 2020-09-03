import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppComponent } from './app.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';

import {MatToolbarModule} from '@angular/material/toolbar';
import {MatIconModule} from '@angular/material/icon';
import {MatSidenavModule} from '@angular/material/sidenav';
import {MatListModule} from '@angular/material/list';
import {MatTabsModule} from '@angular/material/tabs';
import {MatButtonModule} from '@angular/material/button';
import {MatTableModule} from '@angular/material/table';
import {MatCheckboxModule} from '@angular/material/checkbox';
import {MatAutocompleteModule} from '@angular/material/autocomplete';
import {MatInputModule} from '@angular/material/input';
import {MatSortModule} from '@angular/material/sort';
import {MatPaginatorModule} from '@angular/material/paginator';
import { StudentsComponent } from './teacher/students.component';
import { StudentsContComponent } from './teacher/students-cont.component';
import { AppRoutingModule } from './app-routing-module';
import { HomeComponent } from './home.component';
import { PageNotFoundComponent } from './page-not-found.component';
import { VmsContComponent } from './teacher/vms-cont.component';
import { HttpClientModule } from '@angular/common/http';
import { LoginDialogComponent } from './auth/login-dialog.component';
import {MatDialogModule} from '@angular/material/dialog';
import { FormsModule } from '@angular/forms';
import { ReactiveFormsModule } from '@angular/forms';

import { HTTP_INTERCEPTORS } from '@angular/common/http';
import { JwtAuthInterceptor } from './jwt-auth.interceptor';
import { StudentService } from './services/student.service';
import { AuthService } from './auth/auth.service';
import { RegisterComponent } from './auth/register.component';
import { SuccessRegisterDialogComponent } from './auth/success-register-dialog.component';
import { CoursesComponent } from './teacher/courses.component';
import { HomeSidevanComponent } from './home-sidevan.component';
import { CoursesContComponent } from './teacher/courses-cont.component';
import { ProfileComponent } from './profile.component';
import { CsvUploadDialogComponent } from './teacher/csv-upload-dialog.component';


@NgModule({
  declarations: [
    AppComponent,
    StudentsComponent,
    StudentsContComponent,
    HomeComponent,
    PageNotFoundComponent,
    VmsContComponent,
    LoginDialogComponent,
    RegisterComponent,
    SuccessRegisterDialogComponent,
    CoursesComponent,
    HomeSidevanComponent,
    CoursesContComponent,
    ProfileComponent,
    CsvUploadDialogComponent,
  ],
  entryComponents: [
    LoginDialogComponent
  ],
  imports: [
    BrowserModule,
    BrowserAnimationsModule, 
    MatToolbarModule, MatIconModule, MatSidenavModule, 
    MatListModule, MatTabsModule, MatButtonModule, MatTableModule,
    MatCheckboxModule, MatAutocompleteModule, MatInputModule, 
    MatSortModule, MatPaginatorModule, AppRoutingModule, HttpClientModule,
    MatDialogModule, FormsModule, ReactiveFormsModule
  ],
  providers: [{ provide: HTTP_INTERCEPTORS, useClass:JwtAuthInterceptor, multi: true }, StudentService, AuthService],
  bootstrap: [AppComponent]
})
export class AppModule { }
