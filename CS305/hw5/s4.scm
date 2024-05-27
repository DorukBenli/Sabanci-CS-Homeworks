(define s4-interpret (lambda (e)
   (cond 
     ( (number? e) e )
     ( (not (list? e))			(error "s4-interpret cannot evaluate: " e))
     ( (not (eq? (car e) '+)) 		(error "s4-interpret cannot evaluate: " e))
     ( else 				(apply + (map s4-interpret (cdr e)))))))
                                           
                                           


