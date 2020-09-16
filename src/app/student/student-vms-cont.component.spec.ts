import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { StudentVmsContComponent } from './student-vms-cont.component';

describe('StudentVmsContComponent', () => {
  let component: StudentVmsContComponent;
  let fixture: ComponentFixture<StudentVmsContComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ StudentVmsContComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(StudentVmsContComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
