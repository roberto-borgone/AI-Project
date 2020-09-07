import { Component, OnInit, ViewChild, Input, Inject } from '@angular/core';
import { MatTableDataSource } from '@angular/material/table';
import { Student } from '../student.model';
import { MatSort } from '@angular/material/sort';
import { MatPaginator } from '@angular/material/paginator';
import { CsvStudent } from '../csv-student.model';
import { MAT_DIALOG_DATA } from '@angular/material/dialog';
import { DialogData } from '../auth/login-dialog.component';

@Component({
  selector: 'app-csv-upload-dialog',
  templateUrl: './csv-upload-dialog.component.html',
  styleUrls: ['./csv-upload-dialog.component.css']
})
export class CsvUploadDialogComponent implements OnInit {

  dataSource = new MatTableDataSource<CsvStudent>();

  @ViewChild(MatPaginator) paginator: MatPaginator;

  ngAfterViewInit() {
    this.dataSource = new MatTableDataSource<CsvStudent>(this.data);
    this.dataSource.paginator = this.paginator;
  }

  displayedColumns: string[] = ['id', 'name', 'surname', 'email', 'done', 'message'];

  constructor(@Inject(MAT_DIALOG_DATA) public data: CsvStudent[]) {
    console.log("Sono nel costruttore");
    console.log(data);
    this.dataSource = new MatTableDataSource<CsvStudent>(data);

  }

  ngOnInit(): void {
  }

}
