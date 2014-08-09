%% ====================================================================
%%  MyReportClass.cls' - a class for book reports
%%  -------------------------------------------------------------------
%%  Florentin Delaine
%%  florentin.delaine@ens-cachan.fr
%%
%%  -------------------------------------------------------------------
%%  Inspired from LMTClassReport designed by
%%  David Néron
%%  neron@lmt.ens-cachan.fr
%% ====================================================================
\def\fileversion{0.8}
\def\filedate{2014/08/04}
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{MyReportClass}[\filedate\space v\fileversion\space-
                          a class for book reports]
\LoadClass[11pt]{article}

% ---------------------------------------> Chargement des packages <---

\RequirePackage{geometry}          % Régler la taille de la page
\RequirePackage{multicol}
\RequirePackage{subfiles}
\RequirePackage{amsmath,amssymb,bm}% Étendre les fonctions maths
\RequirePackage{fancybox}          % Faire de jolies boîtes
\RequirePackage{calc}              % Faire des calculs
\RequirePackage{xspace}            % Gestion automatique des espaces
\RequirePackage{fancyhdr}          % Configurer headers/footers
\RequirePackage[centerlast,bf]{caption2} % Captions
\RequirePackage[center]{subfigure} % Inclusion de subfigures
\RequirePackage{array}             % Gestion des tableaux
\RequirePackage[french]{babel}    % Règles typo françaises
%\RequirePackage[english]{babel}
%\RequirePackage{soulutf8}
\RequirePackage{ifpdf}             % Tester si génération d'un pdf

\RequirePackage{enumerate}
\RequirePackage{eurosym}
\RequirePackage{fancyref}
\RequirePackage{numprint}
\RequirePackage{amsfonts}
\RequirePackage{makeidx}
\RequirePackage{siunitx}

% ------------------------------------------> Géométrie de la page <---

\geometry{a4paper,
          twoside,
 %         body={15cm,22cm},
          lmargin=2.5cm,
          rmargin=2.5cm,
          footskip=1.5cm,
          top=3cm,
 %         headsep=1cm,
 %         head=15pt
          }

% ---------------------------------------------> LaTeX ou pdfLaTeX <---

\ifpdf
   \RequirePackage[pdftex]{graphicx,color}
   \RequirePackage[pdftex]{hyperref}
   \geometry{pdftex}
\else
   \RequirePackage[dvips]{graphicx,color}
   \RequirePackage[ps2pdf]{hyperref}
   \geometry{dvips}
\fi
	       
% --------------------------------------------> Réglage d'hyperref <---

\hypersetup{a4paper=true,
            pdfstartview=FitH,
	        colorlinks=false,
	        breaklinks=true,
	        pdfborder=0 0 0}
	       
% ----------------------------------------------> siunitx <---
\sisetup{locale = FR}
%\sisetup{locale = US}

% ----------------------------------------------> drawing packages <---
\RequirePackage[europeanresistors]{circuitikz}
\RequirePackage{chemfig}

% ----------------------------------------------> fancyref <---
\renewcommand{\fancyrefdefaultformat}{plain}
%\renewcommand*{\frefeqname}{équation}
%\renewcommand*{\freftabname}{tableau}
\renewcommand*{\fancyrefargdelim}{-}

% ------------------------------------------------> toc et minitoc <---

% Réglage de la toc
\setcounter{tocdepth}{2}       % profondeur de la toc
\setcounter{secnumdepth}{3}    % profondeur de la numérotation 

% --------------------------------------------> Quelques commandes <---

% Pour laisser un espace vertical entre deux paragraphes
\newcommand{\Par}{\bigskip\par}

\parskip 0.5em

% Pour que la page paire, ajoutée afin de commencer les chapitres
% sur une page impaire, soit sans header/footer
\newcommand{\clearemptydoublepage}{\newpage{\pagestyle{empty}%
                                   \cleardoublepage}}
                                   
% thank title
\newcommand{\thanksbloctitle}{
      \vspace*{50\p@}
      \parindent \z@ \raggedleft
     {\huge\bfseries Acknowledgements}
     \interlinepenalty\@M
     \vskip 20\p@
     }
                                   
% Des remerciements
\newenvironment{thanksbloc}
   {%
     \clearemptydoublepage
     \thispagestyle{empty}
     {\thanksbloctitle}
     \vfill 
     }
   {%
     \vfill\clearemptydoublepage}

% Des paragraphes encadrés
\newenvironment{parboxed}
   {%
     \setlength{\fboxsep}{10pt}
     \begin{Sbox}%
     \begin{minipage}{\linewidth-2\fboxrule-2\fboxsep}
     \setlength{\abovedisplayskip}{0pt}
     \setlength{\belowdisplayskip}{0pt}}
   {%
     \end{minipage}%
     \end{Sbox}
     \[\fbox{\TheSbox}\]\aftergroup\ignorespaces}

% subsubsubsection

%\setcounter{secnumdepth}{4}
%\makeatletter
%\newcounter {subsubsubsection}[subsubsection]
%\renewcommand\thesubsubsubsection{\thesubsubsection .\@alph\c@subsubsubsection}
%\newcommand\subsubsubsection{\@startsection{subsubsubsection}{4}{\z@}%
%                                     {-3.25ex\@plus -1ex \@minus -.2ex}%
%                                     {1.5ex \@plus .2ex}%
%                                     {\normalfont\normalsize\bfseries}}
%\renewcommand\paragraph{\@startsection{paragraph}{5}{\z@}%
%                                    {3.25ex \@plus1ex \@minus.2ex}%
%                                    {-1em}%
%                                    {\normalfont\normalsize\bfseries}}
%\renewcommand\subparagraph{\@startsection{subparagraph}{6}{\parindent}%
%                                       {3.25ex \@plus1ex \@minus .2ex}%
%                                       {-1em}%
%                                      {\normalfont\normalsize\bfseries}}
%\newcommand*\l@subsubsubsection{\@dottedtocline{4}{10.0em}{4.1em}}
%\renewcommand*\l@paragraph{\@dottedtocline{5}{10em}{5em}}
%\renewcommand*\l@subparagraph{\@dottedtocline{6}{12em}{6em}}
%\newcommand*{\subsubsubsectionmark}[1]{}
%\makeatother

% -----------------------------> Configuration des headers/footers <---

% Titre de bas de page
\def\lmt@foottitle{}
\newcommand{\foottitle}[1]{\def\lmt@foottitle{#1}}

% Pour une page courante : avec header et footer
\fancyhf{}
\fancyfoot[C]{\small\itshape\lmt@foottitle}
\fancyhead[RO,LE]{\small\bfseries\thepage}
\fancyhead[LO]{\nouppercase{\small\bfseries\rightmark}}
\fancyhead[RE]{\nouppercase{\small\bfseries\leftmark}}
\renewcommand{\footrulewidth}{.5pt}
\renewcommand{\headrulewidth}{.5pt}

% Pour une page de début de chapitre (ie de style plain) :
% avec seulement footer
\fancypagestyle{plain}{
\fancyhf{}
\fancyfoot[C]{\small\itshape\lmt@foottitle}
\renewcommand{\footrulewidth}{.5pt}
\renewcommand{\headrulewidth}{.0pt}}

%%%%%
%\pagestyle{fancy}

%\renewcommand{\chaptermark}[1]{\markboth{#1}{}}
%\renewcommand{\sectionmark}[1]{\markright{#1}}
%
%\pagestyle{headings}
%
%\usepackage{lastpage}
%\usepackage{fancyhdr}
%\pagestyle{fancy}
%
%\renewcommand{\headrulewidth}{0.25mm}
%\fancyhead[L]{\textsc{Commissioning of the new CTPCORE module}}
%%\fancyhead[R]{\lesauteurs}
%
%\renewcommand{\footrulewidth}{0.25mm}
%\fancyfoot[C]{\thepage / \pageref{LastPage}}
%
%\renewcommand{\headrule}{{\color[rgb]{0,0,0}%
%\hrule width\headwidth height\headrulewidth \vskip-\headrulewidth}}
%
%\renewcommand{\footrule}{{\color[rgb]{0,0,0}%
%\vskip-\footruleskip\vskip-\footrulewidth
%\hrule width\headwidth height\footrulewidth\vskip\footruleskip}}


% ------------------------------> Gestion des flottants/références <---

% Noms des flottants
%\newcommand{\Figure}{\textsc{Fig.}}
%\newcommand{\Tableau}{\textsc{Tab.}}
%\newcommand{\Algorithme}{\textsc{Alg.}}
%\newcommand{\Chapitre}{\textsc{Chap.}}
