import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { HomeSidevanComponent } from './home-sidevan.component';

describe('HomeSidevanComponent', () => {
  let component: HomeSidevanComponent;
  let fixture: ComponentFixture<HomeSidevanComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ HomeSidevanComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(HomeSidevanComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
