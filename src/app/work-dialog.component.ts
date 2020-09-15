import { Component, EventEmitter, Inject, OnInit, Output, ViewChild } from '@angular/core';
import { MatDialog, MatDialogConfig, MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { MatPaginator } from '@angular/material/paginator';
import { MatSort } from '@angular/material/sort';
import { MatTableDataSource } from '@angular/material/table';
import { Subscription } from 'rxjs';
import { DialogData } from './auth/login-dialog.component';
import { HistoryDialogComponent } from './history-dialog.component';
import { LastWork } from './last-work.model';
import { AssignmentService } from './services/assignment.service';
import { Work } from './work.model';

@Component({
  selector: 'app-work-dialog',
  templateUrl: './work-dialog.component.html',
  styleUrls: ['./work-dialog.component.css']
})
export class WorkDialogComponent {

  @ViewChild(MatPaginator) 
  paginator: MatPaginator

  displayedColumns: string[] = ['id', 'name', 'surname', 'status', 'timestamp', "voto", 'lode','history'];
  dataSource: MatTableDataSource<LastWork>
  subscriptions: Subscription = new Subscription()

  file: any;

  constructor(public dialogRef: MatDialogRef<WorkDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: LastWork[], private assignmentService: AssignmentService, private dialog: MatDialog) {
      console.log("Sono nel costruttore");
      console.log(data)
      this.dataSource = new MatTableDataSource(data)
      this.dataSource.paginator = this.paginator
     }

  ngAfterViewInit() {
    this.dataSource.paginator = this.paginator;
  }

  applyFilter(event: Event) {
    const filterValue = (event.target as HTMLInputElement).value;
    console.log(filterValue);
    this.dataSource.filter = filterValue.trim().toLowerCase();
  }

  showHistory(assignmentId: number, studentId: string) {
    console.log("showHystory in work-dialog.ts")
    this.subscriptions.add(this.assignmentService.getStudentHistory(assignmentId, studentId).subscribe(history => {
      console.log(history); 

      const dialogConfig = new MatDialogConfig();
      dialogConfig.width = '80%';
      dialogConfig.height = '70%';
      dialogConfig.data = {
        works: history,
        assignmentId: assignmentId,
        studentId: studentId
      }
  
      let diag = this.dialog.open(HistoryDialogComponent, dialogConfig);
  
      this.subscriptions.add(diag.afterClosed().subscribe());
    }));
  }

}
