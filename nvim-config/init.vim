call plug#begin('~/.local/share/nvim/plugged')

Plug 'scrooloose/nerdtree', { 'on':  'NERDTreeToggle' }
Plug 'rust-lang/rust.vim' 
Plug 'sebastianmarkow/deoplete-rust'
Plug 'bling/vim-airline'
Plug 'w0rp/ale'
Plug 'lifepillar/vim-solarized8' 
Plug 'racer-rust/vim-racer', { 'for': 'rust' }
Plug 'tpope/vim-surround'
Plug 'Shougo/deoplete.nvim', { 'do': ':UpdateRemotePlugins'}


" Initialize plugin system
call plug#end()
let g:rustfmt_autosave = 1
let g:deoplete#sources#rust#racer_binary='/home/isaiahp/.cargo/bin/racer'
let g:NERDTreeDirArrowExpandable = '▸'
let g:NERDTreeDirArrowCollapsible = '▾'
set background=dark
colorscheme solarized8


" NERDTree
" Open NERDTree in the directory of the current file (or /home if no file is open)
function! NERDTreeToggleFind()
  if (exists("t:NERDTreeBufName") && bufwinnr(t:NERDTreeBufName) != -1)
    execute ":NERDTreeClose"
  else
    execute ":NERDTreeFind"
  endif
endfunction

set number
" Set Proper Tabs
set tabstop=4
set shiftwidth=4
set smarttab
set expandtab

" Always display the status line
set laststatus=2

" airline
set laststatus=2
let g:airline_left_sep=""
let g:airline_left_alt_sep="|"
let g:airline_right_sep=""
let g:airline_right_alt_sep="|"
let g:airline#extensions#tabline#enabled = 1
let g:airline#extensions#tabline#fnamemod = ':t'
let g:airline#extensions#tabline#show_tab_nr = 1
let g:airline#extensions#tabline#tab_nr_type = 1 " show tab number not number of split panes
let g:airline#extensions#tabline#show_close_button = 0
let g:airline#extensions#tabline#show_buffers = 0
" let g:airline#extensions#hunks#enabled = 0
" let g:airline_section_z = ""

