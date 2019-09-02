! authors   : Giuseppe Piero Brandino - eXact-lab s.r.l.
! date      : October 2018
! copyright : GNU Public License

#define PREC 8

program main
   implicit none

   integer :: n,m,o

   real(PREC), allocatable :: a(:,:), b(:,:), c(:,:)
   character(len=12) ::arg
   real(8) t0, t1

   if ( command_argument_count() .lt. 3) then
      call get_command_argument(0,arg)
      write(*,*) " Calculates c(n,o)=a(n,m)*b(m,o) ";
      write(*,*) " Usage: ", arg , " n m o "
      call exit(1)
   endif

   call get_command_argument(1, arg)
   read(arg, '(I12)') n
   call get_command_argument(2, arg)
   read(arg, '(I12)') m
   call get_command_argument(3, arg)
   read(arg, '(I12)') o

   allocate(a(n,m))
   allocate(b(m,o))
   allocate(c(n,o))

   call setup_matrix(a,n,m,0)
   call setup_matrix(b,m,o,n*m)

   call cpu_time(t0)
   call mat_mult(a,b,c,n,m,o)
   call cpu_time(t1)
   write(*,'(A28,F10.3,A2)') "NON-optimized: Elapsed time ", t1-t0, " s"

!   call cpu_time(t0)
!   call mat_mult_opt(a,b,c,n,m,o)
!   call cpu_time(t1)

!   write(*,'(A28,F10.3,A2)') "    Optimized: Elapsed time ", t1-t0, " s"

!   call cpu_time(t0)
!   call dgemm('n','n',n, o, m, 1.0, a, n, b, m, 0.0, c, n)
!   call cpu_time(t1)
!
!   write(*,'(A28,F10.3,A2)') "       BLAS: Elapsed time ", t1-t0, " s"


   deallocate(a,b,c)
end program



subroutine setup_matrix(a,n,m,stride)
    
    integer, intent(in) :: n,m,stride
    real(8), dimension(n,m) :: a
    
    do i = 1,n
        do j = 1,m
            a(i,j) = (i-1)*m + j + stride    
        end do
    end do
    
end subroutine

subroutine mat_mult(a,b,c,n,m,o)
    
    ! mat mult c(n,o) = a(n,m) * b(m,o)
    
    integer, intent(in) :: n,m,o
    integer i,j,k
    real(PREC), dimension(n,m) :: a
    real(PREC), dimension(m,o) :: b
    real(PREC), dimension(n,o) :: c
    real(PREC) :: acc
    
    do i = 1,n
        do j = 1,o
            do k = 1,m
                c(i,j) = c(i,j) + a(i,k) * b(k,j)
            end do
        end do
    end do
    
end subroutine

subroutine mat_mult_opt(a,b,c,n,m,o)
    !
    ! mat mult c(n,o) = a(n,m) * b(m,o)
    !
    integer, intent(in) :: n,m,o
    integer i,j,k
    real(PREC), dimension(n,m) :: a
    real(PREC), dimension(m,o) :: b
    real(PREC), dimension(n,o) :: c
    real(PREC), dimension(m) :: aux
    real(PREC) :: acc
    
    do j = 1,o
        do k = 1,m
            do i = 1,n
                c(i,j) = c(i,j) + a(i,k) * b(k,j)
            end do
        end do
    end do
 
 
end subroutine
