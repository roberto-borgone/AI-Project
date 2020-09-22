import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { NewOwnerDialogComponent } from './new-owner-dialog.component';

describe('NewOwnerDialogComponent', () => {
  let component: NewOwnerDialogComponent;
  let fixture: ComponentFixture<NewOwnerDialogComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ NewOwnerDialogComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(NewOwnerDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
