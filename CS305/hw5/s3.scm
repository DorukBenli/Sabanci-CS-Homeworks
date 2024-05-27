(define s3-interpret (lambda (e)
   (cond 
     ( (number? e) e )
     ( (not (list? e))			(error "s3-interpret cannot evaluate: " e))
     ( (not (= (length e) 3)) 		(error "s3-interpret cannot evaluate: " e))
     ( (not (eq? (car e) '+)) 		(error "s3-interpret cannot evaluate: " e))
     ( else 				(+ 
                                           (s3-interpret (cadr e))
                                           (s3-interpret (caddr e)))))))


