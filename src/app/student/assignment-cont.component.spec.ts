import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { AssignmentContComponent } from './assignment-cont.component';

describe('AssignmentContComponent', () => {
  let component: AssignmentContComponent;
  let fixture: ComponentFixture<AssignmentContComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ AssignmentContComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(AssignmentContComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
