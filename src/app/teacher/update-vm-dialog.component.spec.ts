import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { UpdateVMDialogComponent } from './update-vm-dialog.component';

describe('UpdateVMDialogComponent', () => {
  let component: UpdateVMDialogComponent;
  let fixture: ComponentFixture<UpdateVMDialogComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ UpdateVMDialogComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(UpdateVMDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
