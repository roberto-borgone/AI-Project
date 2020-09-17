import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { WorksDialogComponent } from './works-dialog.component';

describe('WorksDialogComponent', () => {
  let component: WorksDialogComponent;
  let fixture: ComponentFixture<WorksDialogComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ WorksDialogComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(WorksDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
