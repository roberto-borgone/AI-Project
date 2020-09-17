import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { StudentAssignmentContComponent } from './student-assignment-cont.component';

describe('StudentAssignmentContComponent', () => {
  let component: StudentAssignmentContComponent;
  let fixture: ComponentFixture<StudentAssignmentContComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ StudentAssignmentContComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(StudentAssignmentContComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
