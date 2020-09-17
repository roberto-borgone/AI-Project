import { Component, Inject, OnInit, ViewChild } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { MatPaginator } from '@angular/material/paginator';
import { MatTableDataSource } from '@angular/material/table';
import { LastWork } from '../models/last-work.model';
import { Work } from '../models/work.model';

export interface DialogData {
  works: Work[];
  lastWork: LastWork
}

@Component({
  selector: 'app-works-dialog',
  templateUrl: './works-dialog.component.html',
  styleUrls: ['./works-dialog.component.css']
})
export class WorksDialogComponent {

  @ViewChild(MatPaginator) 
  paginator: MatPaginator

  displayedColumns: string[] = ['timestamp', 'type', 'content'];
  dataSource: MatTableDataSource<Work>

  constructor(public dialogHistory: MatDialogRef<WorksDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData) {
      this.dataSource = new MatTableDataSource(data.works)
      this.dataSource.paginator = this.paginator
    }
    showContent(workId: number) {
    }
}
