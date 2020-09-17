import { Component, Inject, OnInit, Output, ViewChild } from '@angular/core';
import { MatDialog, MatDialogConfig, MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { MatPaginator } from '@angular/material/paginator';
import { MatTableDataSource } from '@angular/material/table';
import { Subscription } from 'rxjs';
import { LastWork } from '../models/last-work.model';
import { AssignmentService } from '../services/assignment.service';
import { HistoryDialogComponent } from './history-dialog.component';

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

  showHistory(lastWork : LastWork) {
    console.log("showHystory in work-dialog.ts")
    console.log(lastWork);
    this.subscriptions.add(this.assignmentService.getStudentHistory(lastWork.consegnaId, lastWork.studentId).subscribe(history => {
      console.log(history); 

      const dialogConfig = new MatDialogConfig();
      dialogConfig.width = '80%';
      dialogConfig.height = '70%';
      dialogConfig.data = {
        works: history,
        lastWork: lastWork       
      }
  
      let diag = this.dialog.open(HistoryDialogComponent, dialogConfig);
  
      this.subscriptions.add(diag.afterClosed().subscribe());
    }));
  }

}
