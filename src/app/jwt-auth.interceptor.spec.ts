import { TestBed } from '@angular/core/testing';

import { JwtAuthInterceptor } from './jwt-auth.interceptor';

describe('JwtAuthInterceptor', () => {
  beforeEach(() => TestBed.configureTestingModule({
    providers: [
      JwtAuthInterceptor
      ]
  }));

  it('should be created', () => {
    const interceptor: JwtAuthInterceptor = TestBed.inject(JwtAuthInterceptor);
    expect(interceptor).toBeTruthy();
  });
});
