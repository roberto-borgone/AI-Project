import { Component, OnInit, ViewChild, Input, Inject } from '@angular/core';
import { MatTableDataSource } from '@angular/material/table';
import { Student } from '../student.model';
import { MatSort } from '@angular/material/sort';
import { MatPaginator } from '@angular/material/paginator';
import { CsvStudent } from '../csv-student.model';
import { MAT_DIALOG_DATA } from '@angular/material/dialog';
import { DialogData } from '../auth/login-dialog.component';

export interface PeriodicElement {
  name: string;
  position: number;
  weight: number;
  symbol: string;
}

const ELEMENT_DATA: PeriodicElement[] = [
  {position: 1, name: 'Hydrogen', weight: 1.0079, symbol: 'H'},
  {position: 2, name: 'Helium', weight: 4.0026, symbol: 'He'},
  {position: 3, name: 'Lithium', weight: 6.941, symbol: 'Li'},
  {position: 4, name: 'Beryllium', weight: 9.0122, symbol: 'Be'},
  {position: 5, name: 'Boron', weight: 10.811, symbol: 'B'},
  {position: 6, name: 'Carbon', weight: 12.0107, symbol: 'C'},
  {position: 7, name: 'Nitrogen', weight: 14.0067, symbol: 'N'},
  {position: 8, name: 'Oxygen', weight: 15.9994, symbol: 'O'},
  {position: 9, name: 'Fluorine', weight: 18.9984, symbol: 'F'},
  {position: 10, name: 'Neon', weight: 20.1797, symbol: 'Ne'},
];

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

  displayedColumns: string[] = ['id', 'name', 'surname', 'email', 'done'];

  constructor(@Inject(MAT_DIALOG_DATA) public data: CsvStudent[]) {
    console.log("Sono nel costruttore");
    console.log(data);
    this.dataSource = new MatTableDataSource<CsvStudent>(data);

  }

  ngOnInit(): void {
  }

}
