<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN" [
<!ENTITY html-ss PUBLIC "-//Norman Walsh//DOCUMENT DocBook HTML Stylesheet//EN" CDATA DSSSL>
<!ENTITY print-ss PUBLIC "-//Norman Walsh//DOCUMENT DocBook Print Stylesheet//EN" CDATA DSSSL>
]>


<style-sheet>


;; $Id$

;; demo file specifications
;; Copyright (C) 2004 Uwe Girlich
;;
;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2 of the License, or
;; (at your option) any later version.
;; 
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;; 
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
;; 
;; Uwe Girlich (uwe@planetquake.com)

;; style sheet customization

<style-specification id="html" use="html-stylesheet">
<style-specification-body>

;; We need a table of contents of full depth with section numbers.
(define %generate-article-toc% #t)
(define (toc-depth nd) 7)
(define %section-autolabel% #t)

</style-specification-body>
</style-specification>


<style-specification id="print" use="print-stylesheet">
<style-specification-body>

(define (article-titlepage-recto-elements)
  ;; note: added pubdate to the default list
  (list (normalize "title")
        (normalize "subtitle")
        (normalize "corpauthor")
        (normalize "authorgroup")
        (normalize "author")
	(normalize "pubdate")
        (normalize "abstract")))

;; We need a table of contents of full depth with section numbers.
(define %generate-article-toc% #t)
(define (toc-depth nd) 7)
(define %section-autolabel% #t)

</style-specification-body>
</style-specification>


<external-specification id="html-stylesheet" document="html-ss">
<external-specification id="print-stylesheet" document="print-ss">


</style-sheet>

