import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { OwnerDialogComponent } from './owner-dialog.component';

describe('OwnerDialogComponent', () => {
  let component: OwnerDialogComponent;
  let fixture: ComponentFixture<OwnerDialogComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ OwnerDialogComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(OwnerDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
