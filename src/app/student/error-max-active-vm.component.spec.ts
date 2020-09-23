import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ErrorMaxActiveVmComponent } from './error-max-active-vm.component';

describe('ErrorMaxActiveVmComponent', () => {
  let component: ErrorMaxActiveVmComponent;
  let fixture: ComponentFixture<ErrorMaxActiveVmComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ErrorMaxActiveVmComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ErrorMaxActiveVmComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
