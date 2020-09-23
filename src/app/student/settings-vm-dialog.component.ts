import { Component, Inject } from '@angular/core';
import { FormControl } from '@angular/forms';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { Router } from '@angular/router';
import { VM } from '../models/vm.model';

export interface DialogData {
  ram: FormControl;
  disk: FormControl;
  vCPU: FormControl;
  invalid: boolean; 
  message: String;
  vm: VM
}

@Component({
  selector: 'app-settings-vm-dialog',
  templateUrl: './settings-vm-dialog.component.html',
  styleUrls: ['./settings-vm-dialog.component.css']
})
export class SettingsVmDialogComponent {

  message: String

  constructor(private router: Router,
    public dialogRef: MatDialogRef<SettingsVmDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData) {
      this.message = data.message
    }

  onNoClick(): void {
    this.dialogRef.close();
  }

  getErrorMessageRAM(ram: FormControl) {
    return 'Inserisci un valore valido per la RAM'
  }

  getErrorMessageDisk(disk: FormControl) {
    return 'Inserisci un valore valido per il disco';
  }

  getErrorMessageVCPU(vCPU: FormControl) {
    return 'Inserisci un valore valido per le VCPU';
  }

  getMessage(){
    return this.message;
  }
}
